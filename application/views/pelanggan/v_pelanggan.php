<div class="content-wrapper">
	<!-- Content Header (Page header) -->
	<div class="content-header">
		<div class="container-fluid">
			<div class="row mb-2">
				<div class="col-sm-6">
					<h1 class="m-0"><?= $title; ?></h1>
				</div><!-- /.col -->
				<div class="col-sm-6">
					<ol class="breadcrumb float-sm-right">
						<li class="breadcrumb-item"><a href="<?= base_url('admin'); ?>">Dashboard</a></li>
						<li class="breadcrumb-item active">Data Pelanggan</li>
					</ol>
				</div><!-- /.col -->
			</div><!-- /.row -->
		</div><!-- /.container-fluid -->
	</div>
	<!-- /.content-header -->

	<!-- Main content -->
	<section class="content">
		<div class="container-fluid">
			<div class="row mb-3">
				<div class="col-lg-4">
					<a href="<?= base_url('pelanggan/add'); ?>" class="btn btn-primary btn-sm"><i class="fa fa-plus"></i> Tambah Pelanggan</a>
				</div>
			</div>

			<?php if ($this->session->flashdata('sukses')) : ?>
				<div class="alert alert-success" role="alert">
					<?= $this->session->flashdata('sukses'); ?>
				</div>
			<?php endif; ?>

			<?php if ($this->session->flashdata('error')) : ?>
				<div class="alert alert-danger" role="alert">
					<?= $this->session->flashdata('error'); ?>
				</div>
			<?php endif; ?>

			<div class="row">
				<div class="col-xl-12">
					<div class="card card-secondary">
						<div class="card-header">
							<h3 class="card-title">Data Pelanggan</h3>
						</div>
						<div class="card-body">
							<div class="table-responsive">
								<table id="example1" class="table table-bordered table-striped">
									<thead>
										<tr>
											<th>No</th>
											<th>Serial Num</th>
											<th>Nama Pelanggan</th>
											<th>Status</th>
											<th>Status Selenoid</th>
											<th>Action</th>
										</tr>
									</thead>
									<tbody>
										<?php foreach ($pelanggan as $i => $dt) : ?>
											<tr>
												<td><?php echo $i + 1; ?></td>
												<td><?php echo $dt->serialNumber; ?></td>
												<td><?php echo $dt->nama; ?></td>
												<td>
													<input class="checkPublish" type="checkbox" data-toggle="toggle" name="status" value="<?= $dt->status; ?>" data-id="<?= $dt->id; ?>" data-col="status" <?= ($dt->status == 'Aktif') ? "checked" : ''; ?> data-on="Aktif" data-off="Tidak Aktif" data-onstyle="success" data-offstyle="danger">
												</td>
												<td>
													<input class="checkPublish" type="checkbox" data-toggle="toggle" name="status" value="<?= $dt->selenoid; ?>" data-id="<?= $dt->id; ?>" data-col="selenoid" <?= ($dt->selenoid == 'ON') ? "checked" : ''; ?> data-on="ON" data-off="OFF" data-onstyle="success" data-offstyle="danger">
												</td>
												<td>
													<div class="btn-group">
														<a href="<?= base_url('pelanggan/edit/' . $dt->id); ?>" class="btn btn-warning btn-sm"><i class="fa fa-edit"></i></a>
														<a href="<?= base_url('pelanggan/delete/' . $dt->id); ?>" class="btn btn-danger btn-sm" onclick="return confirm('Apakah yakin data akan dihapus ?')"><i class="fa fa-trash"></i></a>
													</div>
												</td>
											</tr>
										<?php endforeach; ?>
									</tbody>
								</table>
							</div>
						</div>
					</div>
				</div>
			</div>
		</div>
	</section>
</div>

<script>
	let checkPublish = $('.checkPublish');
	$(checkPublish).each(function(i) {
		$(checkPublish[i]).change(function() {
			const id = $(this).data('id');
			const col = $(this).data('col');
			let status = $(this).val();

			if (col === 'selenoid') {
				if (status == 'OFF') {
					status = 'ON';
				} else {
					status = 'OFF';
				}
			} else {
				if (status == 'Tidak Aktif') {
					status = 'Aktif';
				} else {
					status = 'Tidak Aktif';
				}
			}

			$(this).val(status);

			const data = {
				id,
				status,
				col
			};

			$.ajax({
				url: "<?= base_url('pelanggan/status'); ?>",
				type: 'get',
				data: data,
				dataType: 'json',
				success: function(result) {
					if (result === 'ok') {
						toastr.success('Berhasil diupdate');
					} else {
						toastr.error('Gagal diupdate');
					}
				}
			});
		})
	});
</script>